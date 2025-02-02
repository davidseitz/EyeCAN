import {Card, HStack, Text} from "@chakra-ui/react";
import DropShadowContainer from "@/components/miscellaneous/drop-shadow-container/DropShadowContainer.tsx";
import SelectUnitsDialog from "@/components/dashboard/sidebar/data-collection/SelectUnitsDialog.tsx";
import ArrowButton from "@/components/miscellaneous/ArrowButton.tsx";

function DataCollectionCard() {
    return (
        <DropShadowContainer padding={{paddingX: 5, paddingY: 3}} height={"30%"} margins={{marginY: 8}}>
            <Card.Root backgroundColor="transparent" border="none">
                <Card.Header fontSize="2xl" fontWeight={700} margin={0} padding={0}>
                    Datasets
                </Card.Header>
                <Card.Body lineHeight={1.1} fontSize={10} color="#808080" padding={0}>
                    <Text fontSize="sm" fontWeight="bold" marginBottom={1}>
                        Dataset Title
                    </Text>
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit.
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit.
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit.
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit.
                </Card.Body>
                <Card.Footer padding={0} marginTop={4} justifyContent="space-between">
                    <SelectUnitsDialog/>
                    <HStack>
                        <ArrowButton size="2xs" direction={"left"}/>
                        <ArrowButton size="2xs" direction={"right"}/>
                    </HStack>
                </Card.Footer>
            </Card.Root>
        </DropShadowContainer>
        )
}

export default DataCollectionCard;