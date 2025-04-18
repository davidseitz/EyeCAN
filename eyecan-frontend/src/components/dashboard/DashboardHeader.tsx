import {ColorModeButton} from "@/components/ui/color-mode.tsx";
import {HStack} from "@chakra-ui/react";

function DashboardHeader() {
    return (
        <HStack paddingTop={2} paddingRight={2} width="100%" justifyContent="flex-end">
            <ColorModeButton />
        </HStack>
    )
}

export default DashboardHeader;