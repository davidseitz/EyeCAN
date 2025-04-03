import { VStack} from "@chakra-ui/react";
import AnomalyBadge from "@/components/dashboard/chart/header/anomaly-badge/AnomalyBadge.tsx";
import AnomalySkipper from "@/components/dashboard/chart/header/AnomalySkipper.tsx";

function ChHeader() {
    return (
        <VStack gap={0} margin={0} width="100%" height="14%">
            <AnomalySkipper/>
            <AnomalyBadge/>
        </VStack>
    )
}

export default ChHeader;