import {VStack, Text} from "@chakra-ui/react";
import ApHeader from "@/components/dashboard/knowledgebase/article-preview/ApHeader.tsx";
import {Article} from "@/types/article.ts";

interface ArticlePreviewProps {
    maxLines?: number;
    article: Article;
}

function ArticlePreviewCard({article}: ArticlePreviewProps) {
    return(
        <VStack paddingY={2} gap={0} onClick={()=> console.log(article)}>
            <ApHeader article={article} />
            <Text fontSize="sm" lineHeight={1.1} color="gray.500" textAlign="left">
                {article.content.length > 500 ? article.content.slice(0, 400) + "..." : article.content}
            </Text>
        </VStack>
    )
}

export default ArticlePreviewCard;