import {
    DialogActionTrigger,
    DialogBody, DialogCloseTrigger,
    DialogContent, DialogFooter,
    DialogHeader,
    DialogRoot,
    DialogTitle,
    DialogTrigger
} from "@/components/ui/dialog.tsx";
import {Button, Link,} from "@chakra-ui/react";
import ScrollableContainer from "@/components/miscellaneous/scollbar-container/ScrollableContainer.tsx";
import SelectSignalsForm from "@/components/dashboard/sidebar/data-collection/SelectSignalsForm.tsx";
import {Dataset} from "@/types/dataset.ts";

interface SelectSignalsDialogProps {
    dataset: Dataset;
}

function SelectSignalsDialog({dataset}: SelectSignalsDialogProps) {
    const signals = dataset.signals.map((signal)=>signal.signalName)

    return (
        <DialogRoot placement={"center"}>
            <DialogTrigger asChild>
                <Link fontSize={10} fontWeight="bold" color="#808080">
                    Select Signals
                </Link>
            </DialogTrigger>
            <DialogContent backgroundColor={{base: "#ffffff", _dark: "rgb(9, 9, 11)"}}
                           boxShadow={{base: "none", _dark: "0px 0px 10px 0px rgba(144, 144, 144, 0.5)"}}
                           borderRadius="20px"
            >
                <DialogHeader>
                    <DialogTitle>Select Signals</DialogTitle>
                </DialogHeader>
                <DialogBody >

                    <ScrollableContainer height={"300px"} fade={{show: true, size: 50}}>
                        <SelectSignalsForm signals={signals} selectedSignalValues={dataset.selectedSignalValues}/>
                    </ScrollableContainer>

                </DialogBody>
                <DialogFooter>
                    <DialogActionTrigger asChild>
                        <Button size="sm">Close</Button>
                    </DialogActionTrigger>
                </DialogFooter>
                <DialogCloseTrigger />
            </DialogContent>
        </DialogRoot>
    )
}

export default SelectSignalsDialog;