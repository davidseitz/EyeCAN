import {Heading, HStack} from "@chakra-ui/react";
import {FaPlusCircle} from "react-icons/fa";
import styles from "../../styles.module.css"

interface KbHeaderProps {
    onAdd: () => void;
}

function KbHeader({onAdd}: KbHeaderProps) {
    return (
        <HStack justifyContent="space-between">
            <Heading fontSize="4xl" fontWeight="bold">Knowledge Base</Heading>
            <FaPlusCircle className={styles.btn} onClick={onAdd} size={32}/>
        </HStack>
    )
}

export default KbHeader;