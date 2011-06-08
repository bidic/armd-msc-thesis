package pl.com.mcg.server;

import javax.naming.NamingException;
import javax.servlet.http.HttpSession;

import pl.com.mcg.ejb.remote.RemoteStatefulSBS;

/**
 * The server side implementation of the RPC service.
 */
@SuppressWarnings("serial")
public class GwtStatefulSBSServiceImpl extends AbstractServiceServlet {
	@SuppressWarnings("unchecked")
	@Override
	protected <T> T getServiceBean() throws NamingException {
		HttpSession session = getThreadLocalRequest().getSession();
		Object lookup = session.getAttribute("stateful");
		if (lookup == null) {
			lookup = getInitialContext().lookup(RemoteStatefulSBS.class.getName());
		}
		session.setAttribute("stateful", lookup);

		return (T) lookup;
	}
}
