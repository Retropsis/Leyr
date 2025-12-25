#include "LeyrEditor.h"

#define LOCTEXT_NAMESPACE "FLeyrEditorModule"

void FLeyrEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");		
	{
		{
			TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("Actor", "Leyr", LOCTEXT("Leyr", "Leyr"));
			Section->AddCategory("Leyr");
		}
	}

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FLeyrEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLeyrEditorModule, LeyrEditor)