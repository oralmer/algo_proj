from contextlib import contextmanager


@contextmanager
def session_factory_scope(session_factory):
    """automatically handle errors and commit when done
    takes a session factory and creates, then closes, it's own session"""
    session = session_factory()
    try:
        yield session
        session.commit()
    except Exception as e:
        session.rollback()
        raise e
    finally:
        session.close()


@contextmanager
def session_instance_scope(session):
    """automatically handle errors and commit when done - takes a single session"""
    try:
        yield session
        session.commit()
    except Exception as e:
        session.rollback()
        raise e

