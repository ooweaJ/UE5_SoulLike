#include "UI/InGame/UI_BossStatus.h"
#include "UI_BossStatus.h"

void UUI_BossStatus::SetHP(const float CurrentHP, const float MaxHP)
{
    if (FMath::IsNearlyZero(MaxHP))
    {
        ensure(false);
    }
    const float Percent = CurrentHP / MaxHP;

    HPBar->SetPercent(Percent);

    TargetPercent = Percent;
}

void UUI_BossStatus::SetNameTag(FName InName)
{
    BossName->SetText(FText::FromName(InName));
}

void UUI_BossStatus::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    float CurrentPercent = HPSecond->GetPercent();
    float InterpolatedPercent = FMath::FInterpTo(CurrentPercent, TargetPercent, InDeltaTime, 3.0f);
    HPSecond->SetPercent(InterpolatedPercent);
}
