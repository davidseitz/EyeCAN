import KbHeader from "@/components/dashboard/knowledgebase/KbHeader.tsx";
import ArticlePreview from "@/components/dashboard/knowledgebase/article-preview/ArticlePreview.tsx";
import mockData from "@/components/data/mock-data/MockData.ts";
import {VStack} from "@chakra-ui/react";

function KnowledgeBaseWidget() {
    return (
        <VStack maxHeight="100%" overflowY="hidden">
            <KbHeader/>
            {mockData.articles.map((article) =>
                <ArticlePreview key={article.id} article={article}/>)}
        </VStack>
    )
}


export default KnowledgeBaseWidget;