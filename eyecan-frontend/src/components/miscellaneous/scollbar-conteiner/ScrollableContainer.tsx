import React from 'react';
import styles from "./scrollbar-contianer.module.css"

export interface Fade{
    show: boolean;
    size?: number;
}

interface ScrollableContainerProps {
    children: React.ReactNode;
    fade?: Fade;
    width?: string;
    height?: string;
}

function ScrollableContainer ({ children, width, height, fade }: ScrollableContainerProps)  {
    return (
        <div style={{width, height,}} className={styles.scrollbarContainer}>
            {children}
            {fade?.show && <div style={{height: fade.size, width}} className={styles.scrollableContainerFade}/>}
        </div>

    );
}

export default ScrollableContainer;