import {FaPlusCircle} from "react-icons/fa";
import styles from "@/components/styles.module.css";

interface PlusButtonProps {
    size?: number;
    onClick: () => void;
}

function PlusButton({onClick, size}: PlusButtonProps) {
    return (
        <FaPlusCircle className={styles.btn} onClick={onClick} size={size || 32}/>
    )
}

export default PlusButton;