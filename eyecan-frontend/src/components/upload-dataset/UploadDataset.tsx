import {
    DialogActionTrigger,
    DialogBody,
    DialogCloseTrigger,
    DialogContent,
    DialogFooter,
    DialogHeader,
    DialogRoot,
    DialogTrigger
} from "@/components/ui/dialog.tsx";
import {Button, Separator, VStack} from "@chakra-ui/react";
import FileUploadField from "@/components/upload-dataset/FileUploadField.tsx";
import TitleDescriptionForm from "@/components/upload-dataset/TitleDescriptionForm.tsx";

interface UploadDatasetProps {
    borderStyle: string;
}



function UploadDataset({borderStyle}: UploadDatasetProps) {


    return (
        <DialogRoot placement="center" >
            <DialogTrigger asChild>
                <Button width="100%" borderRadius={10} fontWeight="bold" border={borderStyle} variant="outline" size="sm">
                    Upload Dataset
                </Button>
            </DialogTrigger>
            <DialogContent borderRadius={25}>
                <DialogHeader>
                    <DialogCloseTrigger borderRadius={50}/>
                </DialogHeader>
                <DialogBody>

                    <TitleDescriptionForm/>
                    <Separator colorPalette="black" variant="dashed" marginY={12}/>
                    <VStack alignItems="flex-start" width="100%" gap={2}>
                        <FileUploadField label="Upload .mf4 file:" acceptedFileTypes={['.mf4']}/>
                        <FileUploadField label="Upload .dbc file:" acceptedFileTypes={['.dbc']}/>
                    </VStack>

                </DialogBody>
                <DialogFooter justifyContent="space-around">
                    <DialogActionTrigger asChild>
                        <Button variant="outline" paddingX={8} size="xs" borderRadius={8} borderWidth={1} borderColor="black" marginY={6}>
                            Submit
                        </Button>
                    </DialogActionTrigger>
                </DialogFooter>
            </DialogContent>
        </DialogRoot>
    )
}

export default UploadDataset;