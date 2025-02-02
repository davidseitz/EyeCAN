import {IconButton} from "@chakra-ui/react";
import {TbChevronCompactLeft, TbChevronCompactRight} from "react-icons/tb";

interface ArrowButtonProps {
    direction: "left" | "right";
    color?: string;
    size?: "xs" | "sm" | "md" | "lg" | "xl" | "2xl" | "2xs";
}

function ArrowButton({color, size, direction}: ArrowButtonProps) {
    return (
        <IconButton size={size || "sm"} variant="ghost">
            {direction === "left" ? <TbChevronCompactLeft color={color} /> : <TbChevronCompactRight color={color}/>}
        </IconButton>
    )
}

export default ArrowButton;