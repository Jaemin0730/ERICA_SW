import argparse
import torch
import numpy as np
from tqdm import tqdm
from utils._utils import make_test_data_loader # make_data_loader 대신 make_test_data_loader 임포트
from model import BaseModel
import os # 파일 존재 여부 확인을 위해 추가
from sklearn.metrics import confusion_matrix, classification_report

# 재현성 시드 고정 함수
def seed_everything(seed=42):
    import random
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed(seed)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False

def test(args, data_loader, model):
    true = []
    pred = []

    model.eval() # 모델을 평가 모드로 전환
    with torch.no_grad(): # 역전파 계산 비활성화
        pbar = tqdm(data_loader, desc="Testing") # tqdm 설명 추가
        for i, (x, y) in enumerate(pbar):
            x, y = x.to(args.device), y.to(args.device)

            # 1. 원본 이미지 예측
            outputs_orig = model(x)
            outputs_orig_soft = torch.softmax(outputs_orig, dim=1)

            # 2. 좌우반전 이미지 예측 (TTA)
            x_flip_lr = torch.flip(x, dims=[3]) # 좌우 반전 (dim 3은 width)
            outputs_flip_lr = model(x_flip_lr)
            outputs_flip_lr_soft = torch.softmax(outputs_flip_lr, dim=1)

            # 3. 수직반전 이미지 예측 (TTA)
            x_flip_ud = torch.flip(x, dims=[2]) # 수직 반전 (dim 2는 height)
            outputs_flip_ud = model(x_flip_ud)
            outputs_flip_ud_soft = torch.softmax(outputs_flip_ud, dim=1)
            
            # 4. 좌우+수직 반전 이미지 예측 (TTA)
            x_flip_lrud = torch.flip(x, dims=[2, 3]) # 좌우 및 수직 반전
            outputs_flip_lrud = model(x_flip_lrud)
            outputs_flip_lrud_soft = torch.softmax(outputs_flip_lrud, dim=1)

            # 모든 결과의 소프트맥스 평균 (4가지 TTA)
            #outputs_mean = (outputs_orig_soft + outputs_flip_lr_soft + 
            #                outputs_flip_ud_soft + outputs_flip_lrud_soft) / 4 # 평균 내는 수량 변경
            
            # logit 평균 후 softmax
            logits_mean = (outputs_orig + outputs_flip_lr + outputs_flip_ud + outputs_flip_lrud) / 4
            outputs_mean = torch.softmax(logits_mean, dim=1)


            _, predicted = torch.max(outputs_mean, 1)

            pred.extend(predicted.cpu().numpy())
            true.extend(y.cpu().numpy())

    return np.array(pred), np.array(true)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='2025 DL Term Project')
    # 모델 로드 경로를 best_model.pth로 변경 제안
    parser.add_argument('--model-path', default='checkpoints/best_model_84.79.pth', help="Model's state_dict")
    parser.add_argument('--data', default='data/', type=str, help='data folder')
    parser.add_argument('--num-workers', default=4, type=int, help='number of data loading workers') # num_workers 추가
    parser.add_argument('--seed', default=42, type=int, help='random seed')
    args = parser.parse_args()

    # 재현성 강화
    seed_everything(args.seed)
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    args.device = device
    
    # hyperparameters
    args.batch_size = 32
    
    # Make Data loader and Model
    # make_test_data_loader를 호출하여 테스트 전용 로더를 가져옴
    test_loader = make_test_data_loader(args)

    # instantiate model
    model = BaseModel()
    
    # 모델 파일 존재 여부 확인
    if not os.path.exists(args.model_path):
        print(f"Error: Model file not found at {args.model_path}. Please train your model first.")
        exit() # 모델 파일이 없으면 종료

    model.load_state_dict(torch.load(args.model_path, map_location=device)) # map_location 추가
    model = model.to(device)
    
    # Test The Model
    pred, true = test(args, test_loader, model)
    
    if len(pred) > 0:
        accuracy = (true == pred).sum() / len(pred)
        print("Local Test Accuracy (based on your data loader): {:.5f}".format(accuracy))
    else:
        print("No predictions made. Please check your test dataset and loader.")
    
    if len(pred) > 0:
        accuracy = (true == pred).sum() / len(pred)
        print("\nLocal Test Accuracy: {:.5f}".format(accuracy))
