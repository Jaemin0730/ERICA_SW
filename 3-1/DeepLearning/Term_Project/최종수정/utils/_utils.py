from torchvision import datasets, transforms
import torch
from torch.utils.data import DataLoader, random_split, Subset
import numpy as np
import random

# 학습 데이터 변환
train_transforms = transforms.Compose([
    transforms.RandomResizedCrop(224, scale=(0.7, 1.0)),
    transforms.RandomHorizontalFlip(),
    transforms.RandomVerticalFlip(),
    transforms.RandomRotation(30),
    transforms.ColorJitter(brightness=0.4, contrast=0.4, saturation=0.4, hue=0.1),
    transforms.GaussianBlur(kernel_size=3),
    transforms.ToTensor(),
    transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
])

# 검증 및 테스트 데이터 변환
val_test_transforms = transforms.Compose([
    transforms.Resize(256),
    transforms.CenterCrop(224),
    transforms.ToTensor(),
    transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
])

# CutMix 영역 계산 함수
def rand_bbox(size, lam):
    W = size[2]
    H = size[3]
    cut_rat = np.sqrt(1. - lam)
    cut_w = int(W * cut_rat)
    cut_h = int(H * cut_rat)

    cx = np.random.randint(W)
    cy = np.random.randint(H)

    bbx1 = np.clip(cx - cut_w // 2, 0, W)
    bby1 = np.clip(cy - cut_h // 2, 0, H)
    bbx2 = np.clip(cx + cut_w // 2, 0, W)
    bby2 = np.clip(cy + cut_h // 2, 0, H)

    return bbx1, bby1, bbx2, bby2

# CutMix 전용 collate_fn
def cutmix_collate(batch):
    images, labels = zip(*batch)
    images = torch.stack(images)
    labels = torch.tensor(labels)

    lam = np.random.beta(1.0, 1.0)
    rand_index = torch.randperm(images.size(0))
    target_a = labels
    target_b = labels[rand_index]

    bbx1, bby1, bbx2, bby2 = rand_bbox(images.size(), lam)
    images[:, :, bby1:bby2, bbx1:bbx2] = images[rand_index, :, bby1:bby2, bbx1:bbx2]

    lam = 1 - ((bbx2 - bbx1) * (bby2 - bby1) / (images.size(2) * images.size(3)))

    return images, (target_a, target_b, lam)

# 학습용 데이터로더 생성
def make_data_loader(args, split_ratio=0.8, seed=42):
    np.random.seed(seed)
    torch.manual_seed(seed)

    full_train_dataset = datasets.ImageFolder(root=args.data, transform=train_transforms)
    full_val_dataset = datasets.ImageFolder(root=args.data, transform=val_test_transforms)

    train_size = int(split_ratio * len(full_train_dataset))
    val_size = len(full_train_dataset) - train_size

    indices = list(range(len(full_train_dataset)))
    np.random.shuffle(indices)
    train_indices = indices[:train_size]
    val_indices = indices[train_size:]

    train_dataset = Subset(full_train_dataset, train_indices)
    val_dataset = Subset(full_val_dataset, val_indices)

    # CutMix 적용 여부
    collate_fn = cutmix_collate if getattr(args, "augmentation", "") == "cutmix" else None

    train_loader = DataLoader(train_dataset, batch_size=args.batch_size, shuffle=True,
                            num_workers=args.num_workers, pin_memory=getattr(args, "pin_memory", False),
                            drop_last=True, collate_fn=collate_fn)

    val_loader = DataLoader(val_dataset, batch_size=args.batch_size, shuffle=False,
                            num_workers=args.num_workers, pin_memory=getattr(args, "pin_memory", False),
                            drop_last=False)

    return train_loader, val_loader

# 테스트용 데이터로더 생성
def make_test_data_loader(args):
    test_dataset = datasets.ImageFolder(root=args.data, transform=val_test_transforms)

    test_loader = DataLoader(test_dataset, batch_size=args.batch_size, shuffle=False,
                            num_workers=args.num_workers, pin_memory=getattr(args, "pin_memory", False),
                            drop_last=False)
    return test_loader
