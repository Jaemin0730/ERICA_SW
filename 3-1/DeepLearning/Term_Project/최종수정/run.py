import argparse
import torch
from model import BaseModel
from tqdm import tqdm
from utils._utils import make_test_data_loader
import numpy as np

# 재현성 강화
def seed_everything(seed=42):
    import random
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed(seed)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False

def inference(args, data_loader, model):
    model.eval()
    preds = []

    with torch.no_grad():
        for batch in tqdm(data_loader, desc="Inference"):
            if isinstance(batch, (list, tuple)):
                inputs = batch[0]
            else:
                inputs = batch
            inputs = inputs.to(args.device)

            # TTA (Test Time Augmentation) 적용
            # 1. 원본
            outputs_orig = model(inputs)
            outputs_orig_soft = torch.softmax(outputs_orig, dim=1)

            # 2. 좌우반전
            inputs_flip_lr = torch.flip(inputs, dims=[3])
            outputs_flip_lr = model(inputs_flip_lr)
            outputs_flip_lr_soft = torch.softmax(outputs_flip_lr, dim=1)

            # 3. 상하반전
            inputs_flip_ud = torch.flip(inputs, dims=[2])
            outputs_flip_ud = model(inputs_flip_ud)
            outputs_flip_ud_soft = torch.softmax(outputs_flip_ud, dim=1)

            # 4. 좌우+상하반전
            inputs_flip_lrud = torch.flip(inputs, dims=[2, 3])
            outputs_flip_lrud = model(inputs_flip_lrud)
            outputs_flip_lrud_soft = torch.softmax(outputs_flip_lrud, dim=1)

            # Softmax 평균 (TTA)
            #outputs_mean = (outputs_orig_soft + outputs_flip_lr_soft + 
            #                outputs_flip_ud_soft + outputs_flip_lrud_soft) / 4

            logits_mean = (outputs_orig + outputs_flip_lr + outputs_flip_ud + outputs_flip_lrud) / 4
            outputs_mean = torch.softmax(logits_mean, dim=1)

            _, predicted = torch.max(outputs_mean, 1)
            preds.extend(predicted.cpu().numpy())

    return preds

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='2025 DL Term Project')
    parser.add_argument('--load-model', default='checkpoints/best_model_84.79.pth')
    parser.add_argument('--batch-size', default=32, type=int)
    parser.add_argument('--data', default='data/', type=str)
    parser.add_argument('--num-workers', default=4, type=int)
    parser.add_argument('--seed', default=42, type=int)
    args = parser.parse_args()
    
    # 시드 고정 (재현성)
    seed_everything(args.seed)
    
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    args.device = device

    model = BaseModel()
    model.load_state_dict(torch.load(args.load_model, map_location=device))
    model.to(device)

    test_loader = make_test_data_loader(args)

    preds = inference(args, test_loader, model)

    # result.txt에 한 줄에 하나씩 출력
    with open('result.txt', 'w') as f:
        for pred in preds:
            f.write(f"{pred}\n")
