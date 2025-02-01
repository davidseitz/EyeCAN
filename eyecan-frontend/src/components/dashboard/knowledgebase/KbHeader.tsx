import {Heading, HStack} from "@chakra-ui/react";
import PlusButton from "@/components/dashboard/knowledgebase/PlusButton.tsx";

function KbHeader() {
    return (
        <HStack justifyContent="space-between">
            <Heading fontSize="4xl" fontWeight="bold">Knowledge Base</Heading>
            <PlusButton onClick={() => {}}/>
        </HStack>
    )
}

export default KbHeader;