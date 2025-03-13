import React from 'react';
import styles from "./scrollbar-contianer.module.css"
import {Box} from "@chakra-ui/react";
import {Margins} from "@/types/spacing.ts";

export interface Fade{
    show: boolean;
    size?: number;
}

interface ScrollableContainerProps {
    children: React.ReactNode;
    fade?: Fade;
    width?: string;
    height?: string;
    margins?: Margins;
}

function ScrollableContainer ({ children, width, height, fade, margins }: ScrollableContainerProps)  {
    return (
        <Box style={{width, height,}} className={styles.scrollbarContainer} marginX={margins?.marginX} marginY={margins?.marginY}>
            {children}
            {fade?.show &&
                <Box
                    bg={{
                        base: "linear-gradient(to top, rgba(255, 255, 255, 1) 0%, rgba(255, 255, 255, 0) 100%)",
                        _dark: "linear-gradient(to top, rgba(9, 9, 11, 1) 0%, rgba(0, 0, 0, 0) 100%)"}}
                    style={{height: fade.size, width}} className={styles.scrollableContainerFade}
                />
            }
        </Box>

    );
}

export default ScrollableContainer;