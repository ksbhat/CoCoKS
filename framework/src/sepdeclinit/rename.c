#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "str.h"
#include "memory.h"
#include "rename.h"
#include "infostruct.h"
#include "dbug.h"
#include "copy.h"

/*
char *SEPgenvarname(info *arg_info)
{
	DBUG_ENTER("SEPgetvarname");
	char szPattern[3] = "__";
	char  *name,*clearname;
	static int counter = 0;
	struct varlistname *varseq;
	bool flag;

	while (TRUE)
	{
		flag=FALSE;
		if(counter<10)
		{
			clearname=STRitoa(counter);
			name=STRcat("0",clearname);
			MEMfree(clearname);
			clearname=name;
		}
		name = STRcat(szPattern, name);
		MEMfree(clearname);
		counter = (counter + 1) % NAME_COUNTER_LIMIT;
		if(INFO_VARLISTNAME(arg_info)!=NULL)
		{

			varseq=INFO_VARLISTNAME(arg_info);
			while(varseq!=NULL)
			{

				if(STReq(varseq->id,name))
				{

					flag=TRUE;
				}
				varseq=varseq->next;
			}
			if(!flag)
			{
				DBUG_RETURN(name);
			}
		}
		else
			break;
	}
	DBUG_RETURN(name);
}


node *RENAMEenclosedblock(node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPstatementlist");
	INFO_ENLCOSEDBLOCKCOUNT(arg_info)++;
	ENCLOSEDBLOCK_STATEMENTLIST(arg_node)=TRAVdo(ENCLOSEDBLOCK_STATEMENTLIST(arg_node),arg_info);
	INFO_ENLCOSEDBLOCKCOUNT(arg_info)--;
	DBUG_RETURN(arg_node);
}
*/

node *RENAMEvar(node *arg_node, info *info) {
    DBUG_ENTER("RENAMEvar");
    char *oldref;
    struct varlistname *currlistname=INFO_FORID(info);
    while(currlistname!=NULL)
    {
    	if (STReq(currlistname->previd, VAR_NAME(arg_node))) {
    		oldref = VAR_NAME(arg_node);
    		VAR_NAME(arg_node) = STRcpy(currlistname->id);
    		MEMfree(oldref);
    		break;
    	}
    	currlistname=currlistname->next;
    }
    DBUG_RETURN(arg_node);
}

/*
node *RENAMEforstat (node *arg_node,info *arg_info)
{
	node *forinit,*varnode,*varnodecopy,*vardec,*vardeccopy,*vardeclist,*expr,*assign,*statement;
	node *statementlist;
	char *id;
	char *parentid,*parentprevid;
	DBUG_ENTER("SEPforstat");

	/ *
	 *  the block will have to be traversed first so that the order of
	 * statement assignment is maintained in case of nested for loops
	 *
	 * /
	forinit=FORSTAT_VARDEC(arg_node);
	id=SEPgenvarname(arg_info);
	parentid=INFO_FORID(arg_info);
	parentprevid=INFO_PREVFORID(arg_info);
	INFO_PREVFORID(arg_info)=VAR_NAME(VARDEC_VAR(forinit));
	INFO_FORID(arg_info)=id;

	FORSTAT_BLOCK(arg_node) = TRAVdo(FORSTAT_BLOCK(arg_node), arg_info);


	FORSTAT_VARDEC(arg_node)=TRAVdo(FORSTAT_VARDEC(arg_node),arg_info);
	//if(VARDEC_EXPR(forinit)==NULL)
		//printf("\nEXPR NULL");
	expr=COPYdoCopy(VARDEC_EXPR(forinit));
	expr=TRAVopt(expr,arg_info);

	varnode=TBmakeVar(id,NULL);
	vardec=TBmakeVardec(T_int,NULL,varnode,NULL);
	vardeclist=TBmakeVardeclist(vardec,INFO_LVARDECLIST(arg_info));
	INFO_LVARDECLIST(arg_info)=vardeclist;

	vardeccopy=COPYdoCopy(vardec);

	FORSTAT_VARDEC(arg_node)=vardeccopy;



	varnodecopy=COPYdoCopy(varnode);

	assign=TBmakeAssign(varnodecopy,expr);
	statement=TBmakeStatement(assign);
	INFO_APPENDSTATEMENT(arg_info)=statement;


	FORSTAT_EXPR1(arg_node)=TRAVdo(FORSTAT_EXPR1(arg_node),arg_info);
	FORSTAT_EXPR2(arg_node)=TRAVopt(FORSTAT_EXPR2(arg_node),arg_info);



	INFO_PREVFORID(arg_info)=parentprevid;
	INFO_FORID(arg_info)=parentid;
	freeNode(forinit);
	DBUG_RETURN (arg_node);

}

*/
