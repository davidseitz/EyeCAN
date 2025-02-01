import {VStack, Text} from "@chakra-ui/react";
import ApHeader from "@/components/dashboard/knowledgebase/article-preview/ApHeader.tsx";

function ArticlePreview() {
    return(
        <VStack gap={0}>
            <ApHeader title="Title" date="10.02.2025" author="Leon Eltrich" />
            <Text fontSize="sm" lineHeight={1.1} color="gray.500">Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed euismod, libero quis tincidunt tincidunt, libero massa tincidunt erat, quis tincidunt dolor libero quis turpis. </Text>
        </VStack>
    )
}

export default ArticlePreview;