import {Card, Heading, HStack} from "@chakra-ui/react";
import ApprovalIcon from "@/components/miscellaneous/ApprovalIcon.tsx";
import UnitsCard from "@/components/dashboard/sidebar/filter-collection/UnitsCard.tsx";
import ApplyFilterButton from "@/components/miscellaneous/apply-filter-button/ApplyFilterButton.tsx";
import {Filter} from "@/types/filter.ts";

interface FilterPreviewCardProps {
    filter: Filter;
    datasetUnits: string[];
    textPreviewLength?: number;
    appliedFiltersIds: number[];
    onApplyFilter: (filterId: number) => void;
}

function FilterPreviewCard({filter, datasetUnits, textPreviewLength = 180, appliedFiltersIds, onApplyFilter}: FilterPreviewCardProps) {

    const allUnitsIncluded = filter.units.every(unit => datasetUnits.includes(unit));

    return (
        <Card.Root marginTop={4} backgroundColor="transparent" border="none">
            <Card.Header padding={0}>
                <HStack justifyContent="space-between" alignItems="center">
                    <Heading margin={0} fontSize="xs" fontWeight="800">{filter.title}</Heading>
                    <ApprovalIcon approved={allUnitsIncluded} color={"#808080"}/>
                </HStack>
            </Card.Header>
            <Card.Body lineHeight={1.1} fontSize={10} color="#808080" padding={0}>
                {filter.description.length > textPreviewLength ? filter.description.slice(0, textPreviewLength) + "..." : filter.description}
            </Card.Body>
            <Card.Footer padding={0} paddingTop={2} justifyContent="space-between">
                <UnitsCard text="Filter Units" units={filter.units}/>
                <ApplyFilterButton
                    disabled={!allUnitsIncluded}
                    applicable={allUnitsIncluded}
                    applied={appliedFiltersIds.includes(filter.id)}
                    margins={{marginTop: 10}}
                    onClick={() => onApplyFilter(filter.id)}
                />
            </Card.Footer>
        </Card.Root>
    )
}

export default FilterPreviewCard;