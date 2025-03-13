import {IconButton} from "@chakra-ui/react";
import {TbChevronCompactLeft, TbChevronCompactRight} from "react-icons/tb";

interface ArrowButtonProps {
    direction: "left" | "right";
    color?: string;
    size?: "xs" | "sm" | "md" | "lg" | "xl" | "2xl" | "2xs";
    onClick?: () => void;
}

function ArrowButton({color, size, direction, onClick}: ArrowButtonProps) {
    return (
        <IconButton onClick={onClick} size={size || "sm"} variant="ghost">
            {direction === "left" ? <TbChevronCompactLeft color={color} /> : <TbChevronCompactRight color={color}/>}
        </IconButton>
    )
}

export default ArrowButton;