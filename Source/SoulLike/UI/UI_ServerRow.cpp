#include "UI/UI_ServerRow.h"
#include "UI/UI_ServerMenu.h"
#include "Components/Button.h"

void UUI_ServerRow::Setup(UUI_ServerMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;

	RowButton->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
}

void UUI_ServerRow::OnClicked()
{
	Parent->SetSelectedIndex(Index);
}
