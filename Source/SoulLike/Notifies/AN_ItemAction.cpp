#include "Notifies/AN_ItemAction.h"
#include "Actor/Item/Item.h"

FString UAN_ItemAction::GetNotifyName_Implementation() const
{
	return "ItemAction";
}

void UAN_ItemAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AItem* Item = GetCurrentItem(MeshComp))
	{
		Item->OnItemAction(Num);
	}
}
