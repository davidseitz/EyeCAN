import {Em, Heading, HStack} from "@chakra-ui/react";

interface ApHeaderProps {
    title: string;
    date: string;
    author: string;
}

function ApHeader({title, date, author}: ApHeaderProps) {
    return(
        <HStack width="100%" justifyContent="left" gap={10}>
            <Heading fontWeight="bold">{title}</Heading>
            <HStack gap={4}>
                <Em fontSize="sm"> {date}</Em>
                <Em fontSize="sm">{author}</Em>
            </HStack>

        </HStack>
    )
}

export default ApHeader;