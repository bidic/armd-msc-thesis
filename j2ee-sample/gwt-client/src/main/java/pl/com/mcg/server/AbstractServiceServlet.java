package pl.com.mcg.server;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.Properties;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
import javax.servlet.http.HttpSession;

import com.google.gwt.user.client.rpc.IncompatibleRemoteServiceException;
import com.google.gwt.user.client.rpc.SerializationException;
import com.google.gwt.user.server.rpc.RPC;
import com.google.gwt.user.server.rpc.RPCRequest;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;
import com.sun.appserv.security.ProgrammaticLogin;
import com.sun.enterprise.security.common.ProgrammaticLoginInterface;

@SuppressWarnings("serial")
public abstract class AbstractServiceServlet extends RemoteServiceServlet {
	static {
		System.setProperty("java.security.auth.login.config", "login.conf");
	}

	@Override
	public String processCall(String payload) throws SerializationException {
		Method rpcMethod = null;

		try {
			RPCRequest rpcRequest = RPC.decodeRequest(payload, this.getClass());
			RPC.invokeAndEncodeResponse(this, rpcRequest.getMethod(), rpcRequest.getParameters());
		} catch (IncompatibleRemoteServiceException irse) {
			RPCRequest rpcRequest = RPC.decodeRequest(payload);
			rpcMethod = rpcRequest.getMethod();

			try {
				// try {
				Method[] methods = this.getClass().getDeclaredMethods();
				for (Method method : methods) {
					if ((Modifier.isPublic(method.getModifiers()) || Modifier.isProtected(method.getModifiers()))
							&& method.getReturnType().equals(rpcMethod.getReturnType()) && rpcMethod.getName().equals(method.getName())
							&& matchParameters(rpcMethod, method)) {
						try {
							Object result = method.invoke(this, rpcRequest.getParameters());
							return RPC.encodeResponseForSuccess(rpcMethod, result);
						} catch (Exception e) {
							e.printStackTrace();
							return RPC.encodeResponseForFailure(rpcMethod, e);
						}
					}
				}
				// } catch (Exception e1) {
				// e.printStackTrace();
				// }

				return RPC.invokeAndEncodeResponse(getServiceBean(), rpcRequest.getMethod(), rpcRequest.getParameters());
			} catch (Exception e) {
				e.printStackTrace();
				return RPC.encodeResponseForFailure(rpcMethod, e);
			}
		}

		return RPC.encodeResponseForFailure(rpcMethod, new Exception(
				"Blocked attempt to access interface. Method implementation not found."));
	}

	private boolean matchParameters(Method m1, Method m2) {
		Class<?>[] param1 = m1.getParameterTypes();
		Class<?>[] param2 = m2.getParameterTypes();

		if (param1.length == param2.length) {
			for (int idx = 0; idx < param1.length; idx++) {
				if (!param1[idx].equals(param2[idx])) {
					return false;
				}
			}
		} else {
			return false;
		}

		return true;
	}

	protected abstract <T> T getServiceBean() throws NamingException;

	protected Context getInitialContext() throws NamingException {
		// // props.setProperty(Context.SECURITY_PRINCIPAL, "admin");
		// // props.setProperty(Context.SECURITY_CREDENTIALS, "admin");
		// props.setProperty(Context.URL_PKG_PREFIXES, "com.sun.enterprise.naming");
		// props.setProperty(Context.INITIAL_CONTEXT_FACTORY, "com.sun.enterprise.naming.impl.SerialInitContextFactory");
		// props.setProperty(Context.STATE_FACTORIES, "com.sun.corba.ee.impl.presentation.rmi.JNDIStateFactoryImpl");
		// props.setProperty("org.omg.CORBA.ORBInitialHost", "192.168.1.166");
		// props.setProperty("org.omg.CORBA.ORBInitialPort", "3700");

		Properties props = new Properties();
		FileInputStream stream = null;
		try {
			stream = new FileInputStream("jndi.properties");
			props.load(stream);
			// props.setProperty(Context.SECURITY_PRINCIPAL, "a");
			// props.setProperty(Context.SECURITY_CREDENTIALS, "A1234567");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (stream != null) {
				try {
					stream.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		// HttpSession session = getThreadLocalRequest().getSession();
		// InitialContext ctx = (InitialContext) session.getAttribute("ctx");
		// if (ctx == null) {
		// ctx = new InitialContext(props);
		// }
		// session.setAttribute("ctx", ctx);

		return new InitialContext(props);
	}

	protected Context getAuthorizedContext() {
		HttpSession session = getThreadLocalRequest().getSession();
		String username = (String) session.getAttribute("username");
		String password = (String) session.getAttribute("password");

		Context ctx = null;
		try {
			try {
				if (username != null && password != null) {
					// TODO
					ProgrammaticLoginInterface loginCtx = new ProgrammaticLogin();
					loginCtx.login(username, password, "mcgRealm", true);
					// ORB orb = new GlassFishORBHelper().getORB();

					// loginCtx.login(username, password.toCharArray());
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			ctx = getInitialContext();
		} catch (Exception e) {
			e.printStackTrace();
		}

		return ctx;

	}
}