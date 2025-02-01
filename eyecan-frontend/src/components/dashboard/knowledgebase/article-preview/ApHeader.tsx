import {Em, Heading, HStack} from "@chakra-ui/react";
import {Article} from "@/components/dashboard/knowledgebase/article-preview/ArticlePreview.tsx";

interface ApHeaderProps {
    article: Article;
}

function ApHeader({article}: ApHeaderProps) {
    return(
        <HStack width="100%" justifyContent="left" gap={10}>
            <Heading fontSize="md" fontWeight="bold">{article.title}</Heading>
            <HStack gap={4}>
                <Em fontSize="11px"> {article.date}</Em>
                <Em fontSize="11px">{article.author}</Em>
            </HStack>

        </HStack>
    )
}

export default ApHeader;