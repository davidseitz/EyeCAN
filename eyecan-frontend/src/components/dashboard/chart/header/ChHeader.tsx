import {HStack, Text, VStack} from "@chakra-ui/react";
import AnomalyBadge from "@/components/dashboard/chart/header/anomaly-badge/AnomalyBadge.tsx";
import AnomalySkipper from "@/components/dashboard/chart/header/AnomalySkipper.tsx";

interface ChHeaderProps {
    unit1?: string,
    unit2?: string,
}

function ChHeader({unit1, unit2}: ChHeaderProps) {
    return (
        <VStack gap={0} margin={0} width="100%" height="14%">
            <AnomalySkipper/>
            <HStack justifyContent="space-between" width="100%">
                <Text fontSize="xs" marginBottom="-15px">{unit1}</Text>
                <AnomalyBadge/>
                <Text fontSize="xs" marginBottom="-15px">{unit2}</Text>
            </HStack>
        </VStack>
    )
}

export default ChHeader;