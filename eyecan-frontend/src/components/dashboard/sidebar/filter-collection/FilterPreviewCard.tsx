import {Card, Heading, HStack} from "@chakra-ui/react";
import ApprovalIcon from "@/components/miscellaneous/ApprovalIcon.tsx";
import UnitsCard from "@/components/dashboard/sidebar/filter-collection/UnitsCard.tsx";
import ApplyFilterButton from "@/components/miscellaneous/apply-filter-button/ApplyFilterButton.tsx";

interface FilterPreviewCardProps {
    title: string;
    text: string;
    filterUnits: string[];
    datasetUnits: string[];
    textPreviewLength?: number;
}

function FilterPreviewCard({title, text, filterUnits, datasetUnits, textPreviewLength = 180}: FilterPreviewCardProps) {

    const allUnitsIncluded = filterUnits.every(unit => datasetUnits.includes(unit));

    return (
        <Card.Root marginTop={4} backgroundColor="transparent" border="none" >
            <Card.Header padding={0}>
                <HStack justifyContent="space-between" alignItems="center">
                    <Heading margin={0} fontSize="xs" fontWeight="800">{title}</Heading>
                    <ApprovalIcon approved={allUnitsIncluded} color={"#808080"}/>
                </HStack>
            </Card.Header>
            <Card.Body lineHeight={1.1} fontSize={10} color="#808080" padding={0}>
                {text.length > textPreviewLength ? text.slice(0, textPreviewLength) + "..." : text}
            </Card.Body>
            <Card.Footer padding={0} paddingTop={2} justifyContent="space-between">
                <UnitsCard text="Filter Units" units={filterUnits} />
                <ApplyFilterButton margins={{marginTop: 10}}/>
            </Card.Footer>
        </Card.Root>
    )
}

export default FilterPreviewCard;