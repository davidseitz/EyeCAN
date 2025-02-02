import {
    DialogActionTrigger,
    DialogBody, DialogCloseTrigger,
    DialogContent, DialogFooter,
    DialogHeader,
    DialogRoot,
    DialogTitle,
    DialogTrigger
} from "@/components/ui/dialog.tsx";
import {Button, Link, VStack} from "@chakra-ui/react";
import {Checkbox} from "@/components/ui/checkbox.tsx";
import ScrollableContainer from "@/components/miscellaneous/scollbar-container/ScrollableContainer.tsx";

function SelectUnitsDialog() {
    const units = ["speed", "temp", "angle", "voltage", "current", "frequency", "power", "energy", "resistance", "capacitance", "pressure", "density", "force", "magnetism", "inductance", "luminous flux", "illumination", "length", "mass"];

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
                        <VStack align="start">
                            {units.map((unit, index) => <Checkbox value={unit} key={index}>{unit}</Checkbox>)}
                        </VStack>
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