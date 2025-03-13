import {
    DialogActionTrigger,
    DialogBody, DialogCloseTrigger,
    DialogContent, DialogFooter,
    DialogHeader,
    DialogRoot,
    DialogTitle,
    DialogTrigger
} from "@/components/ui/dialog.tsx";
import {Button} from "@chakra-ui/react";

interface UploadDatasetProps {
    borderStyle: string;
}

function UploadDataset({borderStyle}: UploadDatasetProps) {
    return (
        <DialogRoot placement="center">
            <DialogTrigger asChild>
                <Button width="100%" borderRadius={10} fontWeight="bold" border={borderStyle} variant="outline" size="sm">
                    Upload Dataset
                </Button>
            </DialogTrigger>
            <DialogContent>
                <DialogHeader>
                    <DialogTitle>Dialog Title</DialogTitle>
                </DialogHeader>
                <DialogBody>
                    <p>
                        Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do
                        eiusmod tempor incididunt ut labore et dolore magna aliqua.
                    </p>
                </DialogBody>
                <DialogFooter>
                    <DialogActionTrigger asChild>
                        <Button variant="outline">Cancel</Button>
                    </DialogActionTrigger>
                    <Button>Save</Button>
                </DialogFooter>
                <DialogCloseTrigger />
            </DialogContent>
        </DialogRoot>
    )
}

export default UploadDataset;