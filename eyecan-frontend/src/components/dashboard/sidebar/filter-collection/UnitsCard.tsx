import {List, Box, HoverCardArrow, HoverCardContent, HoverCardRoot, HoverCardTrigger, Link} from "@chakra-ui/react";
import {useState} from "react";

interface UnitsCardProps {
    text: string;
    units: string[];
}

function UnitsCard({units, text}: UnitsCardProps) {
    const [open, setOpen] = useState(false)

    return (
        <Box position="relative" padding={0}>
            <HoverCardRoot size="sm" open={open} onOpenChange={(e) => setOpen(e.open)}>
                <HoverCardTrigger asChild>
                    <Link fontSize={10} fontWeight="bold" color="#808080">
                        {text}
                    </Link>
                </HoverCardTrigger>
                <HoverCardContent width="240px" position="absolute" marginLeft="10px">
                    <HoverCardArrow />
                    <List.Root fontSize={10} color="#808080">
                        {units.map((unit, index) => <List.Item key={index}>{unit}</List.Item>)}
                    </List.Root>
                </HoverCardContent>
            </HoverCardRoot>
        </Box>

    )
}

export default UnitsCard