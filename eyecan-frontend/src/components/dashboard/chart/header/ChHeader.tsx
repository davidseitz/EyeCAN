import { VStack} from "@chakra-ui/react";
import AnomalyBadge from "@/components/dashboard/chart/header/anomaly-badge/AnomalyBadge.tsx";
import AnomalySkipper from "@/components/dashboard/chart/header/AnomalySkipper.tsx";

function ChHeader() {
    return (
        <VStack gap={2} height={"15%"}>
            <AnomalySkipper/>
            <AnomalyBadge/>
        </VStack>
    )
}

export default ChHeader;