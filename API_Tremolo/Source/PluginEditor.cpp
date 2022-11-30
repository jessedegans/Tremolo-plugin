

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

API_API_TremoloAudioProcessorEditor::API_API_TremoloAudioProcessorEditor (API_TremoloAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    const juce::Array<juce::AudioProcessorParameter*>& parameters = processor.getParameters();
    int comboBoxCounter = 0;

    int editorHeight = 2 * editorMargin;
    for (int i = 0; i < parameters.size(); ++i) {
        if (const juce::AudioProcessorParameterWithID* parameter =
                dynamic_cast<juce::AudioProcessorParameterWithID*> (parameters[i])) {

            if (processor.parameters.parameterTypes[i] == "Slider") {
                juce::Slider* aSlider;
                sliders.add (aSlider = new juce::Slider());
                aSlider->setTextValueSuffix (parameter->label);
                aSlider->setTextBoxStyle (juce::Slider::TextBoxLeft,
                                          false,
                                          sliderTextEntryBoxWidth,
                                          sliderTextEntryBoxHeight);

                SliderAttachment* aSliderAttachment;
                sliderAttachments.add (aSliderAttachment =
                    new SliderAttachment (processor.parameters.valueTreeState, parameter->paramID, *aSlider));

                components.add (aSlider);
                editorHeight += sliderHeight;
            }

            //======================================

            else if (processor.parameters.parameterTypes[i] == "ToggleButton") {
                juce::ToggleButton* aButton;
                toggles.add (aButton = new juce::ToggleButton());
                aButton->setToggleState (parameter->getDefaultValue(), juce::dontSendNotification);

                ButtonAttachment* aButtonAttachment;
                buttonAttachments.add (aButtonAttachment =
                    new ButtonAttachment (processor.parameters.valueTreeState, parameter->paramID, *aButton));

                components.add (aButton);
                editorHeight += buttonHeight;
            }

            //======================================

            else if (processor.parameters.parameterTypes[i] == "ComboBox") {
                juce::ComboBox* aComboBox;
                comboBoxes.add (aComboBox = new juce::ComboBox());
                aComboBox->setEditableText (false);
                aComboBox->setJustificationType (juce::Justification::left);
                aComboBox->addItemList (processor.parameters.comboBoxItemLists[comboBoxCounter++], 1);

                ComboBoxAttachment* aComboBoxAttachment;
                comboBoxAttachments.add (aComboBoxAttachment =
                    new ComboBoxAttachment (processor.parameters.valueTreeState, parameter->paramID, *aComboBox));

                components.add (aComboBox);
                editorHeight += comboBoxHeight;
            }

            //======================================

            juce::Label* aLabel;
            labels.add (aLabel = new juce::Label (parameter->name, parameter->name));
            aLabel->attachToComponent (components.getLast(), true);
            addAndMakeVisible (aLabel);

            components.getLast()->setName (parameter->name);
            components.getLast()->setComponentID (parameter->paramID);
            addAndMakeVisible (components.getLast());
        }
    }

    //======================================

    editorHeight += components.size() * editorPadding;
    setSize (editorWidth, editorHeight);
}

API_API_TremoloAudioProcessorEditor::~API_API_TremoloAudioProcessorEditor()
{
}

//==============================================================================

void API_API_TremoloAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void API_API_TremoloAudioProcessorEditor::resized()
{
    juce::Rectangle<int> r = getLocalBounds().reduced (editorMargin);
    r = r.removeFromRight (r.getWidth() - labelWidth);

    for (int i = 0; i < components.size(); ++i) {
        if (juce::Slider* aSlider = dynamic_cast<juce::Slider*> (components[i]))
            components[i]->setBounds (r.removeFromTop (sliderHeight));

        if (juce::ToggleButton* aButton = dynamic_cast<juce::ToggleButton*> (components[i]))
            components[i]->setBounds (r.removeFromTop (buttonHeight));

        if (juce::ComboBox* aComboBox = dynamic_cast<juce::ComboBox*> (components[i]))
            components[i]->setBounds (r.removeFromTop (comboBoxHeight));

        r = r.removeFromBottom (r.getHeight() - editorPadding);
    }
}

//==============================================================================
