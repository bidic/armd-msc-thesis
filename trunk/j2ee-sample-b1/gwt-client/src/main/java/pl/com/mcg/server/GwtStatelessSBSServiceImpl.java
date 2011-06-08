package pl.com.mcg.server;

import javax.naming.NamingException;
import javax.servlet.http.HttpSession;

import pl.com.mcg.ejb.remote.RemoteStatelessSBS;

/**
 * The server side implementation of the RPC service.
 */
@SuppressWarnings("serial")
public class GwtStatelessSBSServiceImpl extends AbstractServiceServlet {
	@SuppressWarnings("unchecked")
	@Override
	protected <T> T getServiceBean() throws NamingException {
		HttpSession session = getThreadLocalRequest().getSession();
		Object lookup = session.getAttribute("stateless");
		if (lookup == null) {
			lookup = getInitialContext().lookup(RemoteStatelessSBS.class.getName());
		}
		session.setAttribute("stateless", lookup);

		return (T) lookup;
	}
}
