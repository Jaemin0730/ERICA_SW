import argparse
import torch
import numpy as np
from tqdm import tqdm
import os
from utils._utils import make_data_loader
from model import BaseModel

# 재현성 강화를 위한 시드 고정
def seed_everything(seed=42):
    import random
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed(seed)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False

# CutMix에 대응한 accuracy 계산 함수
def acc(pred, label):
    if isinstance(label, tuple):  # CutMix용
        target_a, target_b, lam = label
        pred_cls = pred.argmax(dim=-1)
        return (lam * (pred_cls == target_a).float() + (1 - lam) * (pred_cls == target_b).float()).sum().item()
    else:
        pred_cls = pred.argmax(dim=-1)
        return (pred_cls == label).sum().item()

# 검증 루프
def validate(args, data_loader, model, criterion, device):
    model.eval()
    val_losses = []
    val_correct = 0
    total = 0

    with torch.no_grad():
        for inputs, labels in tqdm(data_loader, desc="Validation"):
            inputs, labels = inputs.to(device), labels.to(device)
            outputs = model(inputs)
            loss = criterion(outputs, labels)

            val_losses.append(loss.item())
            _, preds = torch.max(outputs, 1)
            val_correct += (preds == labels).sum().item()
            total += labels.size(0)

    return np.mean(val_losses), val_correct / total

# 학습 루프 (CutMix 적용 가능)
def train(args, train_loader, val_loader, model):
    criterion = torch.nn.CrossEntropyLoss()
    optimizer = torch.optim.AdamW(model.parameters(), lr=args.learning_rate, weight_decay=1e-4)
    scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max=args.epochs)
    device = args.device
    best_val_acc = 0.0
    no_improve_cnt = 0

    os.makedirs(args.save_path, exist_ok=True)
    log_file = open(os.path.join(args.save_path, "train_log.txt"), "w")  # ✅ 로그 기록용

    for epoch in range(args.epochs):
        model.train()
        train_losses = []
        train_correct = 0
        total = 0

        for inputs, labels in tqdm(train_loader, desc=f"Epoch {epoch+1} Training"):
            inputs = inputs.to(device)

            if isinstance(labels, tuple):  # CutMix 처리
                labels = tuple(l.to(device) if not isinstance(l, float) else l for l in labels)
                target_a, target_b, lam = labels
            else:
                labels = labels.to(device)

            optimizer.zero_grad()
            outputs = model(inputs)

            # CutMix loss 계산
            if isinstance(labels, tuple):
                loss = lam * criterion(outputs, target_a) + (1 - lam) * criterion(outputs, target_b)
            else:
                loss = criterion(outputs, labels)

            loss.backward()
            torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=5)
            optimizer.step()

            train_losses.append(loss.item())
            train_correct += acc(outputs, labels)
            total += inputs.size(0)

        epoch_train_loss = np.mean(train_losses)
        epoch_train_acc = train_correct / total

        # 검증
        epoch_val_loss, epoch_val_acc = validate(args, val_loader, model, criterion, device)

        # 로깅 출력 및 저장
        print(f'Epoch {epoch+1}')
        print(f'train_loss : {epoch_train_loss:.4f}, train_acc : {epoch_train_acc * 100:.2f}%')
        print(f'val_loss   : {epoch_val_loss:.4f}, val_acc   : {epoch_val_acc * 100:.2f}%')
        print(f'LR: {optimizer.param_groups[0]["lr"]:.6f}')

        log_file.write(f'{epoch+1},{epoch_train_loss:.4f},{epoch_train_acc:.4f},{epoch_val_loss:.4f},{epoch_val_acc:.4f}\n')

        scheduler.step()

        # 모델 저장
        if epoch_val_acc > best_val_acc:
            best_val_acc = epoch_val_acc
            no_improve_cnt = 0
            torch.save(model.state_dict(), f'{args.save_path}/best_model_{best_val_acc*100:.2f}.pth')
            print(f'--> Best model saved! Val Acc: {best_val_acc * 100:.2f}%')
        
        else:
            no_improve_cnt += 1
            if no_improve_cnt >= args.early_stop:
                print(f"Early stopping at epoch {epoch+1}")
                break

    log_file.close()



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='2025 DL Term Project')

    # 기존 구조 유지하며 확장
    parser.add_argument('--save-path', default='checkpoints/', help="Model's state_dict")
    parser.add_argument('--data', default='data/', type=str, help='data folder')
    parser.add_argument('--num-workers', default=4, type=int, help='number of data loading workers')
    parser.add_argument('--backbone', default='b1', help='efficientnet backbone version (b1, b2, b3, ...)')
    parser.add_argument('--seed', default=42, type=int, help='random seed (재현성)')

    # 확장된 기능
    parser.add_argument('--epochs', default=30, type=int)
    parser.add_argument('--learning-rate', default=0.0005, type=float)
    parser.add_argument('--batch-size', default=32, type=int)
    parser.add_argument('--early-stop', default=5, type=int)
    parser.add_argument('--augmentation', default='', choices=['', 'cutmix'], help='data augmentation method')

    args = parser.parse_args()

    # 디바이스 설정 및 시드 고정
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    args.device = device
    seed_everything(args.seed)

    # 기존 스타일 유지한 출력
    print("==============================")
    print("Save path:", args.save_path)
    print("Data path:", args.data)
    print("Backbone:", args.backbone)
    print("Using Device:", device)
    print("Number of usable GPUs:", torch.cuda.device_count())
    print("Batch_size:", args.batch_size)
    print("Learning Rate:", args.learning_rate)
    print("Epochs:", args.epochs)
    print("Num Workers:", args.num_workers)
    print("Random Seed:", args.seed)
    print("Augmentation:", args.augmentation if args.augmentation else "None")
    print("==============================")

    # 데이터 로더, 모델, 학습 호출
    train_loader, val_loader = make_data_loader(args)
    model = BaseModel().to(device)
    train(args, train_loader, val_loader, model)