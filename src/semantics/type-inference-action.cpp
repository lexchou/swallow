#include "type-inference-action.h"
#include "ast/variables.h"
#include "ast/constant.h"
#include "ast/identifier.h"
USE_SWIFT_NS


void TypeInferenceAction::visitVariables(Variables* node)
{
}
void TypeInferenceAction::visitConstants(Constant* node)
{
    int pairs = node->numPairs();
    for(int i = 0; i < pairs; i++)
    {
        std::pair<Pattern*, Expression*> pair = node->getPair(i);
        Type* type = evaluateType(pair.second);
        if(Identifier* id = dynamic_cast<Identifier*>(pair.first))
        {

        }


    }

}
Type* TypeInferenceAction::evaluateType(Expression* expr)
{
}