import {Box} from "@chakra-ui/react";
import React from "react";
import {useColorModeValue} from "@/components/ui/color-mode.tsx";

interface Margins {
    marginX?: number;
    marginY?: number;
}

interface DropShadowContainerProps {
    children?: React.ReactNode;
    height?: string;
    width?: string;
    margins?: Margins;
}

function DropShadowContainer({children, margins, height = "100%", width = "100%"}: DropShadowContainerProps) {
    return (
        <Box height={height} width={width} paddingY={margins?.marginY} paddingX={margins?.marginX}>
            <Box height="100%" width="100%" shadow="xl" borderRadius="2xl">
                {children}
            </Box>
        </Box>

    )
}

export default DropShadowContainer;