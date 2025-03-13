import {HStack, Text} from "@chakra-ui/react";
import ArrowButton from "@/components/miscellaneous/ArrowButton.tsx";

function AnomalySkipper() {
    return (
        <HStack margin={0}>
            <ArrowButton direction="left" color={"#808080"}/>
            <Text fontWeight="normal" fontSize="smaller">2m 17s - 3m 17s</Text>
            <ArrowButton direction="right" color={"#808080"}/>
        </HStack>
    )
}

export default AnomalySkipper;