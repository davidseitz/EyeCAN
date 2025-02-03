import {Card, HStack, Text} from "@chakra-ui/react";
import DropShadowContainer from "@/components/miscellaneous/drop-shadow-container/DropShadowContainer.tsx";
import SelectSignalsDialog from "@/components/dashboard/sidebar/data-collection/SelectSignalsDialog.tsx";
import ArrowButton from "@/components/miscellaneous/ArrowButton.tsx";
import {Dataset} from "@/types/dataset.ts";

interface DataCollectionCardProps {
    dataset: Dataset;
    onNavigate: (increase: boolean)=>void;
}

function DataCollectionCard({dataset, onNavigate}: DataCollectionCardProps) {

    return (
        <DropShadowContainer padding={{paddingX: 5, paddingY: 3}} height={"30%"} margins={{marginY: 8}}>
            <Card.Root backgroundColor="transparent" border="none">
                <Card.Header fontSize="2xl" fontWeight={700} margin={0} padding={0}>
                    Datasets
                </Card.Header>
                <Card.Body lineHeight={1.1} fontSize={10} color="#808080" padding={0}>
                    <Text fontSize="sm" fontWeight="bold" marginBottom={1}>
                        {dataset.title}
                    </Text>
                    {dataset.description.length > 180 ? dataset.description.slice(0, 180) + "..." : dataset.description}
                </Card.Body>
                <Card.Footer padding={0} marginTop={4} justifyContent="space-between">
                    <SelectSignalsDialog dataset={dataset} />
                    <HStack>
                        <ArrowButton onClick={()=>onNavigate(false)} size="2xs" direction={"left"}/>
                        <ArrowButton onClick={()=>onNavigate(true)} size="2xs" direction={"right"}/>
                    </HStack>
                </Card.Footer>
            </Card.Root>
        </DropShadowContainer>
        )
}

export default DataCollectionCard;