import styles from "./applyFilterButton.module.css"
import {Margins} from "@/types/spacing.ts";

interface ApplyFilterButtonProps {
    margins?: Margins;
}

function ApplyFilterButton({margins}: ApplyFilterButtonProps) {
    return (
        <button style={{
            marginTop: `${margins?.marginTop}px`,
            marginBottom: `${margins?.marginBottom}px`,
            marginLeft: `${margins?.marginLeft}px`,
            marginRight: `${margins?.marginRight}px`,
        }}
                className={styles.applyFilterButton}
        >
            Not Applicable
        </button>
    )
}

export default ApplyFilterButton;