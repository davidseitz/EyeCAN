import DropShadowContainer from "@/components/miscellaneous/drop-shadow-container/DropShadowContainer.tsx";
import UploadDataset from "@/components/upload-dataset/UploadDataset.tsx";
import {VStack} from "@chakra-ui/react";
import CreateFilter from "@/components/create-filter/CreateFilter.tsx";
import {useColorModeValue} from "@/components/ui/color-mode.tsx";

function ActionButtons() {
    const borderStyle = useColorModeValue("1.2pt solid black", "1.5pt solid #808080")

    return (
        <DropShadowContainer height={"15%"} padding={{paddingX: 4}}>
            <VStack height={"100%"} width={"100%"} justifyContent={"center"} alignItems={"center"} gap={4}>
                <CreateFilter borderStyle={borderStyle}/>
                <UploadDataset borderStyle={borderStyle}/>
            </VStack>
        </DropShadowContainer>
    )
}

export default ActionButtons;