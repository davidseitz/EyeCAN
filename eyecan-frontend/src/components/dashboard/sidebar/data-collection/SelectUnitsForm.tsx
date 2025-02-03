import {useController, useForm} from "react-hook-form";
import {CheckboxGroup, Fieldset} from "@chakra-ui/react";
import {Checkbox} from "@/components/ui/checkbox.tsx";
import {useEffect} from "react";

interface SelectUnitsFormProps {
    units: string[];
    selectedUnits: string[];
}

function SelectUnitsForm({units, selectedUnits}: SelectUnitsFormProps) {
    const {control} = useForm()
    const framework = useController({control, name: "units", defaultValue: selectedUnits})

    useEffect(() => {
        console.log(framework.field.value)
    }, [framework.field.value])

    return (
        <Fieldset.Root>
            <CheckboxGroup value={framework.field.value} onValueChange={framework.field.onChange} name={framework.field.name}>
                {units.map((unit, index) =>
                    <Checkbox value={unit} name={unit} key={index}>{unit}</Checkbox>)}
            </CheckboxGroup>
            {console.log(framework.field.value)}
        </Fieldset.Root>
    )
}

export default SelectUnitsForm;