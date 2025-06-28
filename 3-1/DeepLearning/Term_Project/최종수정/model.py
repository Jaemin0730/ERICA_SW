import torch
import torch.nn as nn
import torchvision.models as models

class BaseModel(nn.Module):
    '''
    EfficientNet-b1을 backbone으로 사용하는 모델
    '''
    def __init__(self):
        super(BaseModel, self).__init__()
        self.backbone = models.efficientnet_b1(weights='IMAGENET1K_V1')
        num_features = self.backbone.classifier[1].in_features
        
        # EfficientNet의 분류기 제거
        self.backbone.classifier = nn.Identity()

        # Custom head (두 층 + BatchNorm + Dropout)
        self.classifier = nn.Sequential(
            nn.Linear(num_features, 256),
            nn.BatchNorm1d(256),
            nn.ReLU(),
            nn.Dropout(0.4),

            nn.Linear(256, 20)
        )
        
        # --- Weight initialization ---
        for m in self.classifier:
            if isinstance(m, nn.Linear):
                nn.init.kaiming_normal_(m.weight)
                if m.bias is not None:
                    nn.init.zeros_(m.bias)

    def forward(self, x):
        x = self.backbone(x)
        x = self.classifier(x)
        return x
