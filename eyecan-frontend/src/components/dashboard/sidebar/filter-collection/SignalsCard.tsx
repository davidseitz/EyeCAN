import {useState} from "react";
import {
    HoverCardArrow,
    HoverCardContent,
    HoverCardRoot,
    HoverCardTrigger,
} from "@/components/ui/hover-card"
import { Link, List} from "@chakra-ui/react";

interface SignalsCardProps {
    text: string;
    signals: string[];
}

function SignalsCard({signals, text}: SignalsCardProps) {
    const [open, setOpen] = useState(false)

    return (
        <HoverCardRoot size="sm" open={open} onOpenChange={(e) => setOpen(e.open)}>
            <HoverCardTrigger asChild>
                <Link fontSize={10} fontWeight="bold" color="#808080">
                    {text}
                </Link>
            </HoverCardTrigger>
            <HoverCardContent width="200px">
                <HoverCardArrow />
                <List.Root fontSize={10} color="#808080">
                    {signals.map((signal, index) => <List.Item key={index}>{signal}</List.Item>)}
                </List.Root>
            </HoverCardContent>
        </HoverCardRoot>

    )
}

export default SignalsCard;