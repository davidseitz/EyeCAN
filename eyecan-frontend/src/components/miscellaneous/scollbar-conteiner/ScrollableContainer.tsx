import React from 'react';
import styles from "./scrollbar-contianer.module.css"
interface ScrollableContainerProps {
    children: React.ReactNode;
    width?: string;
    height?: string;
}

function ScrollableContainer ({ children, width, height}: ScrollableContainerProps)  {
    return (
        <div className={styles.scrollbarContainer}
            style={{
                width,
                height,
            }}
        >
            {children}
        </div>
    );
}

export default ScrollableContainer;