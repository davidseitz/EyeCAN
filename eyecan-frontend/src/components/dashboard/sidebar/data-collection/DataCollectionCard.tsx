import {Button, Card, HStack, Text} from "@chakra-ui/react";
import DropShadowContainer from "@/components/miscellaneous/drop-shadow-container/DropShadowContainer.tsx";
import SelectSignalsDialog from "@/components/dashboard/sidebar/data-collection/SelectSignalsDialog.tsx";
import ArrowButton from "@/components/miscellaneous/ArrowButton.tsx";
import {Dataset} from "@/types/dataset.ts";
import ScrollableContainer from "@/components/miscellaneous/scollbar-container/ScrollableContainer.tsx";
import ApplyButton from "@/components/miscellaneous/apply-filter-button/ApplyButton.tsx";
import useSelectedDatasetStore from "@/store.ts";

interface DataCollectionCardProps {
    dataset: Dataset;
    onNavigate: (increase: boolean)=>void;
}

function DataCollectionCard({dataset, onNavigate}: DataCollectionCardProps) {
    const {dataset: selectedDataset, setDataset, resetDataset} = useSelectedDatasetStore()

    return (
        <DropShadowContainer padding={{paddingX: 5, paddingY: 3}} height={"30%"} margins={{marginY: 8}}>
            <Card.Root maxHeight="100%" justifyContent="space-between" backgroundColor="transparent" border="none">
                <Card.Header maxHeight="20%" fontSize="2xl" fontWeight={700} margin={0} padding={0}>
                    <HStack justifyContent="space-between" alignItems="center" margin={0} padding={0}>
                        Datasets
                        <ApplyButton applicable={true}
                                     applied={selectedDataset?.title === dataset.title}
                                     text={{trueState: "Loaded", falseState: "Load Dataset"}}
                                     onClick={dataset.title !== selectedDataset?.title ? () => setDataset(dataset) : () => resetDataset()}                        >
                        </ApplyButton>
                    </HStack>
                </Card.Header>
                <Card.Body maxHeight="60%" overflow="hidden" lineHeight={1.1} fontSize={10} color="#808080" padding={0}>
                    <ScrollableContainer height="100%">
                        <Text fontSize="sm" fontWeight="bold" marginBottom={1}>
                            {dataset.title}
                        </Text>
                        {dataset.description.length > 180 ? dataset.description.slice(0, 180) + "..." : dataset.description}
                    </ScrollableContainer>
                </Card.Body>
                <Card.Footer maxHeight="20%" padding={0} marginTop={4} justifyContent="space-between">
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