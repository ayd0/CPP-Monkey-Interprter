#include "../../include/parser.h"
#include <iostream>

void Parser::nextToken() {
    curToken = peekToken;
    peekToken = l.NextToken();
}

Program Parser::ParseProgram() {
    Program program = Program();
    
    while (curToken.Type != token::EOF_T) {
        Statement *stmt = parseStatement();
        if (stmt != nullptr) {
            program.Statements.push_back(stmt);
        }
        nextToken();
    }

    if (program.Statements.size() > 0) {
        program.isEmpty = false;
    }

    return program;
}

Statement* Parser::parseStatement() {
    if (curToken.Type == token::LET) {
        return parseLetStatement();
    } else if (curToken.Type == token::RETURN) {
        return parseReturnStatement();   
    } else {
        return nullptr;
    }
}

Statement* Parser::parseLetStatement() {
    LetStatement *stmt = new LetStatement(curToken, peekToken);
    nextToken();

    if (!expectPeek(token::ASSIGN)) {
        return nullptr;
    }

    // TODO: skipping expressions until we encounter a semicolon
    while (!curTokenIs(token::SEMICOLON)) {
        nextToken();
    }

    return stmt;
}

Statement* Parser::parseReturnStatement() {
   ReturnStatement *stmt = new ReturnStatement(curToken); 
   nextToken();

   // TODO: skipping expressions until we encounter a semicolon
   while (!curTokenIs(token::SEMICOLON)) {
       nextToken();
   }

   return stmt;
}

bool Parser::curTokenIs(token::TokenType t) {
   return curToken.Type == t; 
}

bool Parser::peekTokenIs(token::TokenType t) {
    return peekToken.Type == t;
}

bool Parser::expectPeek(token::TokenType t) {
    if (peekTokenIs(t)) {
        nextToken();
        return true;
    } else {
        peekError(t);
        return false;
    }
}

std::vector<std::string> Parser::Errors() {
    return errors;
}

void Parser::peekError(token::TokenType t){
    errors.push_back("expected next token to be " + t + 
            ", got " + peekToken.Type + " instead");
}

void Parser::checkParserErrors() {
    std::vector<std::string> errors = Errors();
    
    if (errors.size() == 0) {
        return;
    }

    std::cerr << "ERROR::PARSER: Parser has errors: (" << errors.size() << ")" << std::endl;
    for (std::string err : errors) {
        std::cerr << "Parser error: " << err << std::endl;
    }

    return;
}