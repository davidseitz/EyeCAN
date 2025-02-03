import {Card, Heading, HStack} from "@chakra-ui/react";
import ApprovalIcon from "@/components/miscellaneous/ApprovalIcon.tsx";
import SignalsCard from "@/components/dashboard/sidebar/filter-collection/SignalsCard.tsx";
import ApplyFilterButton from "@/components/miscellaneous/apply-filter-button/ApplyFilterButton.tsx";
import {Filter} from "@/types/filter.ts";

interface FilterPreviewCardProps {
    filter: Filter;
    datasetSignals: string[];
    textPreviewLength?: number;
    appliedFiltersIds: number[];
    onApplyFilter: (filterId: number) => void;
}

function FilterPreviewCard({filter, datasetSignals, textPreviewLength = 180, appliedFiltersIds, onApplyFilter}: FilterPreviewCardProps) {

    const allSignalsIncluded = filter.signals.every(signal => datasetSignals.includes(signal));

    return (
        <Card.Root backgroundColor="transparent" border="none">
            <Card.Header padding={0}>
                <HStack justifyContent="space-between" alignItems="center">
                    <Heading margin={0} fontSize="xs" fontWeight="800">{filter.title}</Heading>
                    <ApprovalIcon approved={allSignalsIncluded} color={"#808080"}/>
                </HStack>
            </Card.Header>
            <Card.Body lineHeight={1.1} fontSize={10} color="#808080" padding={0}>
                {filter.description.length > textPreviewLength ? filter.description.slice(0, textPreviewLength) + "..." : filter.description}
            </Card.Body>
            <Card.Footer padding={0} paddingTop={2} marginBottom={5} justifyContent="space-between">
                <SignalsCard text="Signals" signals={filter.signals}/>
                <ApplyFilterButton
                    disabled={!allSignalsIncluded}
                    applicable={allSignalsIncluded}
                    applied={appliedFiltersIds.includes(filter.id)}
                    margins={{marginTop: 10}}
                    onClick={() => onApplyFilter(filter.id)}
                />
            </Card.Footer>
        </Card.Root>
    )
}

export default FilterPreviewCard;