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
import SelectUnitsForm from "@/components/dashboard/sidebar/data-collection/SelectUnitsForm.tsx";
import {Dataset} from "@/types/dataset.ts";

interface SelectUnitsDialogProps {
    dataset: Dataset;
}

function SelectUnitsDialog({dataset}: SelectUnitsDialogProps) {
    const units = dataset.signals.map((signal)=>signal.signalName)

    return (
        <DialogRoot placement={"center"}>
            <DialogTrigger asChild>
                <Link fontSize={10} fontWeight="bold" color="#808080">
                    Select Units
                </Link>
            </DialogTrigger>
            <DialogContent backgroundColor={{base: "#ffffff", _dark: "rgb(9, 9, 11)"}}
                           boxShadow={{base: "none", _dark: "0px 0px 10px 0px rgba(144, 144, 144, 0.5)"}}
                           borderRadius="20px"
            >
                <DialogHeader>
                    <DialogTitle>Select Units</DialogTitle>
                </DialogHeader>
                <DialogBody >

                    <ScrollableContainer height={"300px"} fade={{show: true, size: 50}}>
                        <SelectUnitsForm units={units} selectedUnits={dataset.selectedSignalValues}/>
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

export default SelectUnitsDialog;