import {Box} from "@chakra-ui/react";
import React from "react";
import {useColorModeValue} from "@/components/ui/color-mode.tsx";
import {Margins, Padding} from "@/types/spacing.ts";

interface DropShadowContainerProps {
    children?: React.ReactNode;
    height?: string;
    width?: string;
    margins?: Margins;
    padding?: Padding;
}

function DropShadowContainer ({children, margins, padding, height = "100%", width = "100%"}: DropShadowContainerProps) {
    const boxShadow = useColorModeValue("", "0px 0px 10px 0px rgba(140, 140, 140, 0.5)")
    return (
        <Box height={height} width={width} paddingY={margins?.marginY} paddingX={margins?.marginX}>
            <Box height="100%" width="100%" style={{boxShadow: boxShadow}} paddingX={padding?.paddingX} paddingY={padding?.paddingY} shadow="xl" borderRadius="2xl">
                {children}
            </Box>
        </Box>

    )
}

export default DropShadowContainer;