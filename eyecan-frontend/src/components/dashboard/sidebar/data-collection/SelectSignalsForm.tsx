import {useController, useForm} from "react-hook-form";
import {CheckboxGroup, Fieldset} from "@chakra-ui/react";
import {Checkbox} from "@/components/ui/checkbox.tsx";
import {useEffect} from "react";

interface SelectSignalsFormProps {
    signals: string[];
    selectedSignalValues: string[];
}

function SelectSignalsForm({signals, selectedSignalValues}: SelectSignalsFormProps) {
    const {control} = useForm()
    const framework = useController({control, name: "signals", defaultValue: selectedSignalValues})

    useEffect(() => {
        console.log(framework.field.value)
    }, [framework.field.value])

    return (
        <Fieldset.Root>
            <CheckboxGroup value={framework.field.value} onValueChange={framework.field.onChange} name={framework.field.name}>
                {signals.map((signal, index) =>
                    <Checkbox value={signal} name={signal} key={index}>{signal}</Checkbox>)}
            </CheckboxGroup>
            {console.log(framework.field.value)}
        </Fieldset.Root>
    )
}

export default SelectSignalsForm;