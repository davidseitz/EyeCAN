import {Heading, HStack} from "@chakra-ui/react";
import PlusButton from "@/components/miscellaneous/PlusButton.tsx";

function KbHeader() {
    return (
        <HStack marginBottom={2} width="100%" justifyContent="space-between">
            <Heading fontSize="3xl" fontWeight="bold">Knowledge Base</Heading>
            <PlusButton onClick={() => {}}/>
        </HStack>
    )
}

export default KbHeader;