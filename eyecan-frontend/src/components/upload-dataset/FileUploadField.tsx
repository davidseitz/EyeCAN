import {FileInput, FileUploadClearTrigger, FileUploadLabel, FileUploadRoot} from "@/components/ui/file-upload.tsx";
import {InputGroup} from "@/components/ui/input-group.tsx";
import {LuFileUp} from "react-icons/lu";
import {CloseButton} from "@/components/ui/close-button.tsx";

interface FileUploadFieldProps {
    label: string;
    acceptedFileTypes?: string[];
}

function FileUploadField({label, acceptedFileTypes}: FileUploadFieldProps) {
    return (
        <FileUploadRoot gap="1" maxFiles={1} maxFileSize={100000000} accept={acceptedFileTypes}>
            <FileUploadLabel fontWeight="bold" fontSize="10px" marginBottom="-1">{label}</FileUploadLabel>
            <InputGroup
                w="full"
                startElement={<LuFileUp />}
                endElement={
                    <FileUploadClearTrigger asChild>
                        <CloseButton
                            me="-1"
                            size="xs"
                            variant="plain"
                            focusVisibleRing="inside"
                            focusRingWidth="2px"
                            pointerEvents="auto"
                            color="fg.subtle"
                        />
                    </FileUploadClearTrigger>
                }
            >
                <FileInput borderColor={{base: "black", _dark: "white"}} size="xs" borderRadius={8}/>
            </InputGroup>
        </FileUploadRoot>
    )
}

export default FileUploadField;