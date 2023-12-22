#include "EffectProcessorWrapper.h"
#include <msclr/marshal_cppstd.h>

namespace AmpProcessingWrapper {
    EffectProcessorWrapper::EffectProcessorWrapper(AmpProcessing::Effects::IEffectProcessor* processor) 
        : m_EffectProcessor(processor) {}

    EffectProcessorWrapper::~EffectProcessorWrapper() { }

    EffectProcessorWrapper::!EffectProcessorWrapper() { }

    void EffectProcessorWrapper::ToggleCanProcess()
    {
        m_EffectProcessor->ToggleCanProcess();
    }

    bool EffectProcessorWrapper::CanProcess::get()
    {
        return m_EffectProcessor->GetCanProcess();
    }

    System::String^ EffectProcessorWrapper::Name::get()
    {
        return msclr::interop::marshal_as<System::String^>(m_EffectProcessor->GetName());
    }

    AmpProcessing::Effects::EffectCategory EffectProcessorWrapper::Category::get()
    {
        return m_EffectProcessor->GetCategory();
    }
}
