import styles from "./applyFilterButton.module.css"
import {Margins} from "@/types/spacing.ts";

interface ApplyFilterButtonProps {
    applied?: boolean;
    applicable: boolean; // TODO Make this own interface
    disabled?: boolean;
    margins?: Margins;
    onClick?: () => void;
}

function ApplyFilterButton({margins, applied, applicable, disabled, onClick}: ApplyFilterButtonProps) {
    return (
        <button disabled={disabled} style={{
            marginTop: `${margins?.marginTop}px`,
            marginBottom: `${margins?.marginBottom}px`,
            marginLeft: `${margins?.marginLeft}px`,
            marginRight: `${margins?.marginRight}px`,
        }}
                className={styles.applyFilterButton}
                onClick={onClick}
        >
            {applicable ? applied ? "Applied" : "Not Applied" : "Not Applicable"}
        </button>
    )
}

export default ApplyFilterButton;