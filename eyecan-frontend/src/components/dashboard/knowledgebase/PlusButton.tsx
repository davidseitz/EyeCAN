import {FaPlusCircle} from "react-icons/fa";
import styles from "@/components/styles.module.css";

interface PlusButtonProps {
    size?: number;
    onClick: () => void;
}

// TODO Move to misc

function PlusButton({onClick, size}: PlusButtonProps) {
    return (
        <FaPlusCircle className={styles.btn} onClick={onClick} size={size || 28}/>
    )
}

export default PlusButton;