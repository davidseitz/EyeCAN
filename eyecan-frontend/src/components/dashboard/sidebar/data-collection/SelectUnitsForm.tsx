import {useController, useForm} from "react-hook-form";
import {CheckboxGroup, Fieldset} from "@chakra-ui/react";
import {Checkbox} from "@/components/ui/checkbox.tsx";

interface SelectUnitsFormProps {
    units: string[]
}

function SelectUnitsForm({units}: SelectUnitsFormProps) {
    const {control} = useForm()

    const framework = useController({control, name: "units", defaultValue: [],})

    return (
        <Fieldset.Root>
            <CheckboxGroup value={framework.field.value} onValueChange={framework.field.onChange} name={framework.field.name}>
                {units.map((unit, index) =>
                    <Checkbox value={unit} name={unit} key={index}>{unit}</Checkbox>)}
            </CheckboxGroup>
        </Fieldset.Root>
    )
}

export default SelectUnitsForm;