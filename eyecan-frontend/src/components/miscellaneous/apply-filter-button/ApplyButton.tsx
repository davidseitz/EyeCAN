import styles from "./applyFilterButton.module.css"
import {Margins} from "@/types/spacing.ts";

interface ApplyButtonProps {
    applied?: boolean;
    applicable: boolean; // TODO Make this own interface
    disabled?: boolean;
    margins?: Margins;
    onClick?: () => void;
    text?: ButtonStateTexts;
}

interface ButtonStateTexts{
    trueState?: string;
    falseState?: string;
    unavailableState?: string;
}

function ApplyButton({margins, applied, applicable, disabled, onClick, text}: ApplyButtonProps) {
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
            {applicable ? applied ? text?.trueState : text?.falseState : text?.unavailableState}
        </button>
    )
}

export default ApplyButton;