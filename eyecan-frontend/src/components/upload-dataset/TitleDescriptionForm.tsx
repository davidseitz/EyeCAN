import {Field} from "@/components/ui/field.tsx";
import {Input, Textarea, VStack} from "@chakra-ui/react";
import {useForm} from "react-hook-form";

interface FormValues {
    datasetTitle: string
    bio: string
}

function TitleDescriptionForm() {

    const {
        register,
       // handleSubmit,
        formState: { errors },
    } = useForm<FormValues>()

    return (
        <VStack color="black" gap={2}>
            <Field
                invalid={!!errors.datasetTitle}
                errorText={errors.datasetTitle?.message}
            >
                <label htmlFor="datasetTitle" style={{ marginBottom: '-6px', display: 'block', fontWeight: 'bold', fontSize: '10px'}}>
                    Dataset Title:
                </label>
                <Input
                    id="datasetTitle"
                    borderRadius={8}
                    borderColor="black"
                    {...register("datasetTitle", { required: "Dataset title is required" })}
                />
            </Field>
            <Field
                invalid={!!errors.bio}
                errorText={errors.bio?.message}
            >
                <label htmlFor="bio" style={{ marginBottom: '-6px', display: 'block', fontWeight: 'bold', fontSize: '10px' }}>
                    Dataset Description:
                </label>
                <Textarea
                    maxH="100px"
                    height="100px"
                    borderRadius={8}
                    borderColor="black"
                    {...register("bio", { required: "Bio is required" })}
                />
            </Field>
        </VStack>
    )
}

export default TitleDescriptionForm;