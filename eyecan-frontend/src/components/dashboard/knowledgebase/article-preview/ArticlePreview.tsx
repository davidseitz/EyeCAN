import {VStack, Text} from "@chakra-ui/react";
import ApHeader from "@/components/dashboard/knowledgebase/article-preview/ApHeader.tsx";

interface ArticlePreviewProps {
    maxLines?: number;
    article: Article;
}

export interface Article{
    title: string;
    date: string;
    author: string;
    content: string;
}

function ArticlePreview({article}: ArticlePreviewProps) {
    return(
        <VStack paddingY={2} gap={0}>
            <ApHeader article={article} />
            <Text fontSize="sm" lineHeight={1.1} color="gray.500">
                {article.content.length > 500 ? article.content.slice(0, 400) + "..." : article.content}
            </Text>
        </VStack>
    )
}

export default ArticlePreview;